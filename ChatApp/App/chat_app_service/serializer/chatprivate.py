from rest_framework import serializers

from chat_app_service.models import ChatPrivate


class ChatPrivateSerializer(serializers.ModelSerializer):
    class Meta:
        model = ChatPrivate
        fields = "__all__"
